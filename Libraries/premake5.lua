make_proj('ASL', './ASL')
	excludes { './ASL/Tests/**.cpp' }

make_proj('TestASL', './ASL/Tests', 'Test')
	links { 'ASL' }

make_proj('GUI', './LibGUI')
	links { 'ASL' }
	filter "system:linux"
		links { 'X11' }

make_proj('Codec', './LibCodec')
	links { 'ASL' }
	excludes { './LibCodec/Tests/**.cpp' }

make_proj('TestCodec', './LibCodec/Tests', 'Test')
	links { 'ASL', 'Codec' }
