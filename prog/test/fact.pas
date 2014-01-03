const n=6;
var a:integer;
function fact():integer;
    begin
        if a>1 then
        begin
            a:=a-1;
            fact := (a+1)*fact()
        end
        else
            fact := 1
    end;
begin
    a:=n;
    write(fact())
end.
