make_proj('ASL')
	filter "system:windows"
		links { 'Dbghelp' }
	make_proj('TestASL', './ASL/Tests', 'Test')
		links { 'ASL' }

make_proj('LibCodec')
	links { 'ASL', 'LibCrypto' }
	make_proj('TestCodec', './LibCodec/Tests', 'Test')
		links { 'ASL', 'LibCodec', 'LibCrypto' }

make_proj('LibCrypto')
	links { 'ASL' }
	make_proj('TestCrypto', './LibCrypto/Tests', 'Test')
		links { 'ASL', 'LibCrypto' }

make_proj('LibGUI')
	links { 'ASL' }
	filter "system:linux"
		links { 'X11' }
		files { './LibGUI/Platform/X11/*.h', './LibGUI/Platform/X11/*.cpp' }
	filter "system:windows"
		files { './LibGUI/Platform/Windows/*.h', './LibGUI/Platform/Windows/*.cpp' }

make_proj('LibJS')
	links { 'ASL' }
