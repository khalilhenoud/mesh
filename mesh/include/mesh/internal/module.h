#if !defined(MESH_API)
	#define MESH_API /* NOTHING */

	#if defined(WIN32) || defined(WIN64)
		#undef MESH_API
		#if defined(mesh_EXPORTS)
			#define MESH_API __declspec(dllexport)
		#else
			#define MESH_API __declspec(dllimport)
		#endif
	#endif // defined(WIN32) || defined(WIN64)

#endif // !defined(MESH_API)

