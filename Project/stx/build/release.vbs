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
			ver = ((ver / 100) + 1) * 100
			ver_str = Replace(Space(length - Len(ver)) & ver, Space(1), zero)
			temp_text = Replace(temp_text,temp_arr(2), ver_str )
		end if
		chk = InStr(1, temp_text,"#define STX_VERSION_STR ", 1)
		if chk > 0 then
			dim reg
			set reg = new RegExp
			With reg
				.Pattern ="""([^""]*)"""
				.Global=True
			End With
			
			dim match
			dim matches
			
			set matches = reg.Execute(temp_text)
			for each match in matches
				ver = reg.Replace(temp_text,"$1")
			next
			temp_arr = Split(Split(ver)(2),"_")
			temp_text = Replace(temp_text,CInt(temp_arr(1)),CInt(temp_arr(1)) + 1)
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