.First.lib <- function(lib, pkg)
{
 ##  library.dynam("Rwave", pkg, lib)
  library.dynam("WINRPACK", pkg, lib)
  
  cat("WINRPACK is loaded\n")
}
