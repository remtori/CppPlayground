make_proj('ASL')
	withTest()
	filter "system:windows"
		links { 'Dbghelp' }

make_proj('Reme')
	links { 'ASL', 'GLFW' }
	files
	{
		'Reme/Events/*.h',
		'Reme/Events/*.cpp',
		'Reme/Platform/*.h',
		'Reme/Platform/*.cpp',
	}