var a:array[5] of integer;
	c,d:integer;
procedure swap(var a,b:integer);
var c:integer;
begin
	c := a;
	a := b;
	b := c
end;
begin
	read(c);
	read(d);
	a[1] := c;
	a[2] := d;
	swap(a[1],a[2]);
	write(a[1]);
	write(a[2])
end.
