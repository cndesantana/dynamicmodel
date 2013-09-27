% function [mean_a var_a]=NvsBP()
function [exponentes mat]=NvsBP()
 [status, result]=system('ls SOC_Parameters_*.dat');
 result=regexprep(result,'\n',' ');
 result=regexp(result,'\S+','match');
 sp=[];
 seed=[];
 for i=[1:length(result)],
  r=sscanf(result{i},'SOC_Parameters_sp_%d_seed_%d_real_1.dat');
  sp(end+1)=r(1);
  seed(end+1)=r(2);
 end
 
 sp=sort(unique(sp));        %species existentes en los archivos...
 seed=sort(unique(seed));    %seeds correspondientes a los set de archs...
 
%  Ahora, P/C seed y P/C sitio estudio los parámetros de las especies...

%ALE: abro un archivo solo para saber cuantos sitios hay...
 arch=sprintf('SOC_Parameters_sp_%03d_seed_%d_real_1.dat',sp(1),seed(1)); 
 fid=fopen(arch,'r');
 params=fscanf(fid,'{%f,%f,%f,%f,%d} ',[5 inf]);    %bp,dp,migProb,ndp,numOfInds at each site...
 fclose(fid);
 cantSites=length(params);
%ALE: ahora se cuantos sitios hay...
%  cantSites=100;

 mat=zeros(length(sp),length(seed),2,cantSites);    %mat(species,sitio,seed,x,dato) --> x=1 ==> dato=columna de num of inds (uno por sitio) 
                                                                                    %   x=2 ==> dato=columna de bp          (para todos los sitios)
 exponentes=[];
%  ix_sp=1;
%  ix_seed=1;
 for ix_seed=[1:length(seed)],
%  for ix_seed=[1:1],
  for ix_sp=[1:length(sp)],
   arch=sprintf('SOC_Parameters_sp_%03d_seed_%d_real_1.dat',sp(ix_sp),seed(ix_seed));

  %  d=load(arch,'-regexp','\S+','match')
   fid=fopen(arch,'r');
   params=fscanf(fid,'{%f,%f,%f,%f,%d} ',[5 inf]);    %bp,dp,migProb,ndp,numOfInds at each site...
   fclose(fid);
   mat(ix_sp,ix_seed,1,:)=params(5,:);  %n of inds...
   mat(ix_sp,ix_seed,2,:)=params(1,:);  %Bp
  end
%   for ix_site=[1:cantSites],
%    disp([seed(ix_seed) ix_site]);
%    
% %    
% %    if isempty(find(isnan(mat(:,ix_seed,2,ix_site)))),  %se hace el fit si no hay ningún NaN en la lista de Bp.
% %     rtaFIT=ezfit(mat(:,ix_seed,2,ix_site),mat(:,ix_seed,1,ix_site),'power;log');
% %     if rtaFIT.r>0.5,
% %      exponentes(end+1)=rtaFIT.m(2);
% %     end
% %    end
%   end
 end

 for ix_sp=[1:length(sp)],
  for ix_seed=[1:length(seed)],
   disp([ix_sp seed(ix_seed)]);
   B_aver=mean(mat(ix_sp,ix_seed,2,:));
   B_maxDif=max(abs(mat(ix_sp,ix_seed,2,:)-B_aver));
   hist(mat(ix_sp,ix_seed,2,:)-B_aver,[-1:0.01:1]);
   pause;
  end
 end
 
%  for ix_sp=[1:length(sp)],
%   disp(ix_sp);
%   hist(squeeze(mat(ix_sp,:,2,:)),[0:0.01:1]);
%   pause;
%  end
 
%  disp('Plotting N vs Bp for each site for each seed');
%  disp('Assessing exponent "a"  [N~Bp^a]');
%  
% %  disp({'<a>=', mean(exponentes), 'var(a)=', var(exponentes)});
%  mean_a=mean(exponentes);
%  var_a=var(exponentes);
%  disp(sprintf('<a>=%f \t var(a)=%f\n',mean_a,var_a));


 
 
 
 
%  plot(squeeze(m(:,1,1,1)),squeeze(m(:,1,2,1)))
%  ix_site=1;
%  rta=ezfit(mat(:,1,1,ix_site),mat(:,1,2,ix_site),'power;log');
 
 






%  [status, result]=system('./NvsBP_from_FoodWebNet.pl 17');
%  res=regexprep(result,'\n',' ');
%  r=regexp(res,'\S+\s+','match');
%  
%  N_Bp=zeros(length(r)/2,2);
%  for i=[1:length(r)/2],
%   N_Bp(i,:)=[str2num(r{(i-1)*2+1}) str2num(r{(i-1)*2+2})];
%  end
