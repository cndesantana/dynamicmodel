function cantExtincted=countExtinctions()
 cantExtincted=[];
 [stat, filesAverInd]=system('ls AverIndInTime*.dat');
 filesAverInd=regexprep(filesAverInd,'\n',' ');
 listAverInd=regexp(filesAverInd,'\S+','match');
 for ix=[1:length(listAverInd)],
%   disp(sprintf('tail -1 %s', listAverInd{ix}));
  [stat, cantSPstr]=system(sprintf('tail -1 %s', listAverInd{ix}));
  cantSPstr=regexprep(cantSPstr,'\n',' ');
%   cantSP=regexp(cantSPstr,'\d+','match')
  cantSP=sscanf(cantSPstr,'%d');
  cantExtincted(end+1)=length(find(cantSP==0));
 end
%  [stat,result]=system(tail -1 