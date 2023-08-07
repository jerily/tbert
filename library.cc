#include "bert.h"
#include "library.h"
#include <cstdlib>

#ifndef UNUSED
# define UNUSED(x) UNUSED_ ## x __attribute__((__unused__))
#endif

#ifdef DEBUG
# define DBG(x) x
#else
# define DBG(x)
#endif

#define CheckArgs(min,max,n,msg) \
                 if ((objc < min) || (objc >max)) { \
                     Tcl_WrongNumArgs(interp, n, objv, msg); \
                     return TCL_ERROR; \
                 }

typedef int32_t bert_vocab_id;

static Tcl_HashTable tbert_NameToInternal_HT;
static Tcl_Mutex     tbert_NameToInternal_HT_Mutex;
static int           tbert_ModuleInitialized;
static int
tbert_RegisterName(const char *name, struct bert_ctx *ctx) {

    Tcl_HashEntry *entryPtr;
    int newEntry;

    Tcl_MutexLock(&tbert_NameToInternal_HT_Mutex);
    entryPtr = Tcl_CreateHashEntry(&tbert_NameToInternal_HT, (char*) name, &newEntry);
    if (newEntry) {
        Tcl_SetHashValue(entryPtr, (ClientData)ctx);
    }
    Tcl_MutexUnlock(&tbert_NameToInternal_HT_Mutex);

    DBG(fprintf(stderr, "--> RegisterName: name=%s ctx=%p %s\n", name, ctx, newEntry ? "entered into" : "already in"));

    return 0;
}

static struct bert_ctx *
tbert_GetInternalFromName(const char *name) {
    struct bert_ctx *ctx = NULL;
    Tcl_HashEntry *entryPtr;

    Tcl_MutexLock(&tbert_NameToInternal_HT_Mutex);
    entryPtr = Tcl_FindHashEntry(&tbert_NameToInternal_HT, (char*)name);
    if (entryPtr != NULL) {
        ctx = (struct bert_ctx *)Tcl_GetHashValue(entryPtr);
    }
    Tcl_MutexUnlock(&tbert_NameToInternal_HT_Mutex);

    return ctx;
}

static int tbert_LoadModelCmd(ClientData  UNUSED(clientData), Tcl_Interp *interp, int objc, Tcl_Obj * const objv[] ) {
    DBG(fprintf(stderr,"LoadModelCmd\n"));

    CheckArgs(3,3,1,"handle_name filename");

    struct bert_ctx *ctx = bert_load_from_file(Tcl_GetString(objv[2]));
    if (ctx == NULL) {
        Tcl_SetObjResult(interp, Tcl_NewStringObj("failed to load model", -1));
        return TCL_ERROR;
    }
    tbert_RegisterName(Tcl_GetString(objv[1]), ctx);
    return TCL_OK;
}

static int tbert_UnloadModelCmd(ClientData  UNUSED(clientData), Tcl_Interp *interp, int objc, Tcl_Obj * const objv[] ) {
    DBG(fprintf(stderr,"UnloadModelCmd\n"));

    CheckArgs(2,2,1,"handle_name");
    struct bert_ctx *ctx = tbert_GetInternalFromName(Tcl_GetString(objv[1]));
    bert_free(ctx);
    return TCL_OK;
}

static int tbert_GetVectorCmd(ClientData UNUSED(clientData), Tcl_Interp *interp, int objc, Tcl_Obj * const objv[] ) {
    DBG(fprintf(stderr,"GetVectorCmd\n"));

    CheckArgs(3,4,1,"handle_name text ?n_threads?");

    int n_threads = 1;
    if (objc == 4) {
        Tcl_GetInt(interp, Tcl_GetString(objv[3]), &n_threads);
    }

    struct bert_ctx *ctx = tbert_GetInternalFromName(Tcl_GetString(objv[1]));
    if (ctx == NULL) {
        Tcl_SetObjResult(interp, Tcl_NewStringObj("model not loaded", -1));
        return TCL_ERROR;
    }

    int N = bert_n_max_tokens(ctx);
    // tokenize the prompt
    //bert_vocab_id tokens[N];
    bert_vocab_id *tokens = (bert_vocab_id *)malloc(sizeof(bert_vocab_id)*(size_t)N);

    int n_tokens;
    bert_tokenize(ctx, Tcl_GetString(objv[2]), tokens, &n_tokens, N);

    DBG(
        fprintf(stderr, "[");
            for (int i = 0; i < n_tokens; i++) {
            fprintf(stderr, "%d, ", tokens[i]);
        }
        fprintf(stderr, "]\n");
    );

    DBG(
        for (int i = 0; i < n_tokens; i++) {
            fprintf(stderr, "%d -> %s\n", tokens[i], bert_vocab_id_to_token(ctx, tokens[i]));
        }
    );

    int n_embd = bert_n_embd(ctx);
    //float embeddings[n_embd];
    float *embeddings = (float *)malloc(sizeof(float)*(size_t)n_embd);
    bert_eval(ctx, n_threads, tokens, n_tokens, embeddings);

    DBG(
        for (int i = 0; i < n_embd; i++) {
            fprintf(stderr, "%1.4f ", embeddings[i]);
        }
    );

    Tcl_Obj *result = Tcl_NewObj();
    for (int i = 0; i < n_embd; i++) {
        Tcl_ListObjAppendElement(interp, result, Tcl_NewDoubleObj(embeddings[i]));
    }
    Tcl_SetObjResult(interp, result);
    free(embeddings);
    free(tokens);
    return TCL_OK;
}


static void tbert_ExitHandler(ClientData UNUSED(unused))
{
    Tcl_MutexLock(&tbert_NameToInternal_HT_Mutex);
    Tcl_DeleteHashTable(&tbert_NameToInternal_HT);
    Tcl_MutexUnlock(&tbert_NameToInternal_HT_Mutex);

}


static void tbert_InitModule() {
    Tcl_MutexLock(&tbert_NameToInternal_HT_Mutex);
    if (!tbert_ModuleInitialized) {
        Tcl_InitHashTable(&tbert_NameToInternal_HT, TCL_STRING_KEYS);
        Tcl_CreateThreadExitHandler(tbert_ExitHandler, NULL);
        tbert_ModuleInitialized = 1;
    }
    Tcl_MutexUnlock(&tbert_NameToInternal_HT_Mutex);
}

int Tbert_Init(Tcl_Interp *interp) {
    if (Tcl_InitStubs(interp, "8.6", 0) == NULL) {
        return TCL_ERROR;
    }

    tbert_InitModule();

    Tcl_CreateNamespace(interp, "::tbert", NULL, NULL);
    Tcl_CreateObjCommand(interp, "::tbert::load_model", tbert_LoadModelCmd, NULL, NULL);
    Tcl_CreateObjCommand(interp, "::tbert::unload_model", tbert_UnloadModelCmd, NULL, NULL);
    Tcl_CreateObjCommand(interp, "::tbert::ev", tbert_GetVectorCmd, NULL, NULL);

    return Tcl_PkgProvide(interp, "tbert", "0.1");
}

#ifdef USE_NAVISERVER
int Ns_ModuleInit(const char *server, const char *UNUSED(module)) {
    Ns_TclRegisterTrace(server, (Ns_TclTraceProc *) Tbert_Init, server, NS_TCL_TRACE_CREATE);
    return NS_OK;
}
#endif
