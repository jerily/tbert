set dir [file dirname [info script]]

package ifneeded tbert 0.1 [list load [file join $dir build libtbert.so]]
