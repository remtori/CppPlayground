make_proj('TestASL', './ASL/Tests', 'Test')
	links { 'ASL', }

make_proj('ASL', './ASL')
	excludes { './ASL/Tests/**.h', './ASL/Tests/**.cpp' }

make_proj('LibPlatform', './LibPlatform')
	links { 'ASL' }
	filter "system:linux"
		links { 'X11' }

make_proj('LibPlatform', './LibPlatform')
	links { 'ASL' }
	filter "system:linux"
		links { 'X11' }
