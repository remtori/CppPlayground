make_proj('ASL', './ASL')
	filter "system:windows"
		links { 'Dbghelp' }
	make_proj('TestASL', './ASL/Tests', 'Test')
		links { 'ASL' }

make_proj('Codec', './LibCodec')
	links { 'ASL', 'Crypto' }
	make_proj('TestCodec', './LibCodec/Tests', 'Test')
		links { 'ASL', 'Codec', 'Crypto' }

make_proj('Crypto', './LibCrypto')
	links { 'ASL' }
	make_proj('TestCrypto', './LibCrypto/Tests', 'Test')
		links { 'ASL', 'Crypto' }


make_proj('GUI', './LibGUI')
	links { 'ASL' }
	filter "system:linux"
		links { 'X11' }
		files { './LibGUI/Platform/X11/*.h', './LibGUI/Platform/X11/*.cpp' }
	filter "system:windows"
		files { './LibGUI/Platform/Windows/*.h', './LibGUI/Platform/Windows/*.cpp' }

make_proj('JS', './LibJS')
	links { 'ASL' }
