const two = 2, four = 4, h = 'h', e = 'e', l = 'l', o = 'o';
var x:integer;
    c:char;
function f(p:integer):integer;
	procedure print();
	begin
		write(h);
		write(e);
		write(l);
		write(l);
		write(o);
		write("  ");
		write(p+p);
		write("  ");
	end;
	begin
		print();
		f := p * 2;
	end;
begin
	write(f(2)+h);
	write("  ");
	x := two + 1;
	write("x = ",x);
	write("  ",two+four);
end.
	
	
