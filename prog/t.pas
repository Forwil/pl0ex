const a = 1,b = 'a', c = -122;
var x,y:integer;
	z:char;
function gcd2(var a,b:integer):integer;
var t:integer;
begin
	repeat
		begin
		t := b;
		b := a - a / b * b;
		a := t
		end
	until b = 0;
	gcd2 := a
end;
function gcd1(a,b:integer):integer;
begin
	if b=0 then
		gcd1 :=a
	else
		gcd1 := gcd1(b,a - a/b*b)
end;
begin
	write("shabi");
	read(x);
	read(y);
	write(gcd1(x,y));
	write(gcd2(x,y))
end.
