if(TFEL_DIR)
  set(TFEL_MODULE_PATH ${TFEL_DIR})
else(TFEL_DIR)
  if(TFELHOME)
    set(TFEL_MODULE_PATH ${TFELHOME})
  else(TFELHOME)
    if(DEFINED ENV{TFELHOME})
	  set(TFEL_MODULE_PATH "$ENV{TFELHOME}")      
    endif(DEFINED ENV{TFELHOME})
  endif(TFELHOME)
endif(TFEL_DIR)
