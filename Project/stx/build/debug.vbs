option explicit

Wscript.echo "increment patch version"

dim args
dim fso
dim stream

dim text
dim temp_text
dim temp_join
dim temp_arr
dim ver

dim length
length = 6
dim zero
zero = 0

dim ver_str

dim chk

set args = Wscript.arguments
set fso = CreateObject("Scripting.FileSystemObject")

Wscript.echo args(0)

if args.count > 0 then
	set stream = fso.OpenTextFile(args(0), 1)
	
	do until stream.AtEndOfStream = true
		temp_text = stream.ReadLine
		chk = InStr(1, temp_text,"#define STX_VERSION ", 1)
		if chk > 0 then
			temp_arr = Split(temp_text)
			ver = CInt(temp_arr(2))
			ver = ver + 1
			ver_str = Replace(Space(length - Len(ver)) & ver, Space(1), zero)
			temp_text = Replace(temp_text,temp_arr(2), ver_str )
		end if

		temp_join = text
		text = temp_join & temp_text & vbCrLf
	loop
	
	stream.Close
	set stream = fso.OpenTextFile(args(0), 2, true)
	
	
	stream.Write(text)
	

	stream.Close
	
end if

set stream = Nothing
set fso = Nothing