function N_Bp=NvsBP()
 [status, result]=system('./NvsBP_from_FoodWebNet.pl 17');
 res=regexprep(result,'\n',' ');
 r=regexp(res,'\S+\s+','match');
 
 N_Bp=zeros(length(r)/2,2);
 for i=[1:length(r)/2],
  N_Bp(i,:)=[str2num(r{(i-1)*2+1}) str2num(r{(i-1)*2+2})];
 end
