package require tbert

if { [llength $argv] != 1 } {
    puts "Usage: $argv0 <model>"
    puts "Example: $argv0 /path/to/all-MiniLM-L12-v2/ggml-model-q4_0.bin"
    exit 1
}

#set filename [file join $dir .. "models/all-mpnet-base-v2/ggml-model-f32.bin"]
#set filename [file join $dir .. "models/all-MiniLM-L12-v2/ggml-model-q4_0.bin"]

set filename [lindex $argv 0]
tbert::load_model mymodel $filename
set embeddings [tbert::ev mymodel "hello world"]
puts \n\nembeddings=$embeddings
puts \n\n[llength $embeddings]
tbert::unload_model mymodel