var a,b:integer;
procedure swap(var a,b:integer);
var c:integer;
begin
	c := a;
	a := b;
	b := c
end;
begin
	read(a);
	read(b);
	swap(a,b);
	write(a);
	write(b);
end.
