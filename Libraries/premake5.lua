make_proj('TestASL', './ASL/Tests', 'Test')
	links { 'ASL', }

make_proj('ASL', './ASL')
	excludes { './ASL/Tests/**.h', './ASL/Tests/**.cpp' }
	filter "system:windows"
		links { 'Dbghelp' }
