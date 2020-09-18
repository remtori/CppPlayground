make_proj('TestV8', 'ConsoleApp')
	libdirs
	{
		_MAIN_SCRIPT_DIR .. '/third_party/v8',
	}
	links { 'v8', 'pthread', 'ASL' }

make_proj('CppHidden', 'ConsoleApp')

make_proj('WebSocketServer', 'ConsoleApp')
	links { 'pthread' }