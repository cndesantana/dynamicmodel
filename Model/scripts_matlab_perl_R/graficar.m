function graficar(m,ix_seed, ix_site)
%  l=3;
%  subplot(l,l,mod(ix_site,l^2));
 undofit;
 loglog(m(:,ix_seed,2,ix_site),m(:,ix_seed,1,ix_site),'o');showfit('power;log'); 
