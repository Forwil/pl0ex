var headarray,footarray:array[3] of integer;
    i: integer;
    head,foot:integer;
procedure calculation(var head,foot:integer);
  var n,cock,rabbit:integer;
  begin
    n:=0;
    for cock:=1 to head do
    begin
      rabbit:=head-cock;
      if cock*2+rabbit*4=foot then
      begin
        write(" cocks: ,cock");
        write(" rabbits: ", rabbit);
        n:=n+1
      end
    end;
    if n=0 then 
       begin
         write(" cocks: ",0);
        write(" rabbits: ", 0)
       end
  end;
begin
  for i:=0 to 2 do
  begin
   if i=0 then 
    begin
      head:=4;
      foot:=8
    end;
    if i=1 then 
    begin
      head:=8;
      foot:=4
    end;  
    if i=2 then 
    begin
      head:=3;
      foot:=5
    end; 
    headarray[i]:= head;
    footarray[i]:=foot;
    calculation(headarray[i],footarray[i])
  end
end.
