%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                                         %
%                            Plot Amdahl's law                            %
%                                                                         %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

clc;
clear all;
close all;

bf_1e4 = 4.52e-4;
bf_1e5 = 4.26e-5;
bf_1e6 = 3.56e-6;

bh_1e4 = 3.34e-2;
bh_1e5 = 9.23e-3;
bh_1e6 = 7.99e-3;


speedup = @(alpha, p) 1/(alpha + (1-alpha)/p);

k=1;
for i=1:100
    nodes(k) = i;
    sp_bf_1e4(k) = speedup(bf_1e4, i);
    sp_bf_1e5(k) = speedup(bf_1e5, i);
    sp_bf_1e6(k) = speedup(bf_1e6, i);
    sp_bh_1e4(k) = speedup(bh_1e4, i);
    sp_bh_1e5(k) = speedup(bh_1e5, i);
    sp_bh_1e6(k) = speedup(bh_1e6, i);
    k = k+1;
end

figure()
hold all;
plot(nodes, sp_bf_1e4, 'k', 'LineWidth', 1.2);
plot(nodes, sp_bf_1e5, 'k--', 'LineWidth', 1.2);
plot(nodes, sp_bf_1e6, 'k:', 'LineWidth', 1.5);
plot(nodes, sp_bh_1e4, 'b', 'LineWidth', 1.2);
plot(nodes, sp_bh_1e5, 'b--', 'LineWidth', 1.2);
plot(nodes, sp_bh_1e6, 'b:', 'LineWidth', 1.5);
legend('Brute-force - 10^4 bodies', 'Brute-force - 10^5 bodies', 'Brute-force - 10^6 bodies', ...
    'Barnes-Hut - 10^4 bodies', 'Barnes-Hut - 10^5 bodies', 'Barnes-Hut - 10^6 bodies', 'Location', 'northwest')
xlabel('Number of nodes');
ylabel('Theoretical speedup');
grid on;
    