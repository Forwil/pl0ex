var a:array[100]of integer;
	n,i:integer;
begin
	read(n);
	a[1] := 1;
	a[2] := 2;
	for i:=3 to n do
	begin
		a[i] := a[i-1] + a[i-2];
		write(a[i])
	end
end.
