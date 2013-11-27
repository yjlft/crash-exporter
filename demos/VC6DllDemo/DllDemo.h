

#ifdef DLLDEMO_EXPORTS
#define DLLDEMO_API __declspec(dllexport)
#else
#define DLLDEMO_API __declspec(dllimport)
#endif


DLLDEMO_API void Unknown();
DLLDEMO_API void SEH();
DLLDEMO_API void RaiseException();
DLLDEMO_API void ThrowException();

 



