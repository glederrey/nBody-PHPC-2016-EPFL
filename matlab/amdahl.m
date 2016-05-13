%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                                         %
%                            Plot Amdahl's law                            %
%                                                                         %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

clc;
clear all;
close all;

frac_bf = 0.00058;
frac_bh = 4.66e-3;


speedup = @(alpha, p) 1/(alpha + (1-alpha)/p);

k=1;
for i=1:5:1000
    nodes(k) = i;
    sp_bf(k) = speedup(frac_bf, i);
    sp_bh(k) = speedup(frac_bh, i);
    k = k+1;
end

figure()
hold all;
plot(nodes, sp_bf);
plot(nodes, sp_bh);
    