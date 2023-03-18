#ifdef ADD_EXPORTS
  #define ADDAPI __declspec(dllexport)
#else
  #define ADDAPI __declspec(dllimport)
#endif