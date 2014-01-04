var i:integer;
	a:array[5] of integer;
begin
	for i:=0 to 4 do
		begin
			a[i]:=i;
		end;
	for i:=4 downto 0 do
		begin
			write(a[i])
		end
end.
