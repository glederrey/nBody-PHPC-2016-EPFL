%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                                         %
%                            Plot Amdahl's law                            %
%                                                                         %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

clc;
clear all;
close all;

filename = '../results/bf_1e4_time.dat';
parallel = false;
algo = 'brute-force';

[total, simulation, loading, iteration, building, communication] = time(filename, algo, parallel);

display('Brute-Force - 1e4 bodies');

display(['Total time: ', num2str(total), ' [s]']);
display(['Loading time: ', num2str(loading), ' [s]']);
display(['Mean iteration time: ', num2str(mean(iteration)), ' [s]']);
display(['Max iteration time: ', num2str(max(iteration)), ' [s]']);
display(['Min iteration time: ', num2str(min(iteration)), ' [s]']);
bf_1e4 = loading/(100*mean(iteration));
display(['Proportion of serial time on 100 iterations: ', num2str(bf_1e4)]);
display(['Proportion of serial time on 1000 iterations: ', num2str(loading/(1000*mean(iteration)))]);
display('\n');

filename = '../results/bf_1e5_time.dat';
[total, simulation, loading, iteration, building, communication] = time(filename, algo, parallel);

display('Brute-Force - 1e5 bodies');

display(['Total time: ', num2str(total), ' [s]']);
display(['Loading time: ', num2str(loading), ' [s]']);
display(['Mean iteration time: ', num2str(mean(iteration)), ' [s]']);
display(['Max iteration time: ', num2str(max(iteration)), ' [s]']);
display(['Min iteration time: ', num2str(min(iteration)), ' [s]']);
bf_1e5 = loading/(100*mean(iteration));
display(['Proportion of serial time on 100 iterations: ', num2str(bf_1e5)]);
display(['Proportion of serial time on 1000 iterations: ', num2str(loading/(1000*mean(iteration)))]);
display('\n');

filename = '../results/bf_1e6_time.dat';
[total, simulation, loading, iteration, building, communication] = time(filename, algo, parallel);

display('Brute-Force - 1e6 bodies');

display(['Total time: ', num2str(total), ' [s]']);
display(['Loading time: ', num2str(loading), ' [s]']);
display(['Mean iteration time: ', num2str(mean(iteration)), ' [s]']);
display(['Max iteration time: ', num2str(max(iteration)), ' [s]']);
display(['Min iteration time: ', num2str(min(iteration)), ' [s]']);
bf_1e6 = loading/(100*mean(iteration));
display(['Proportion of serial time on 100 iterations: ', num2str(bf_1e6)]);
display(['Proportion of serial time on 1000 iterations: ', num2str(loading/(1000*mean(iteration)))]);
display('\n');

filename = '../results/bh_1e4_time.dat';
parallel = false;
algo = 'barnes-hut';

[total, simulation, loading, iteration, building, communication] = time(filename, algo, parallel);

display('Barnes-Hut - 1e4 bodies');

display(['Total time: ', num2str(total), ' [s]']);
display(['Loading time: ', num2str(loading), ' [s]']);
display(['Mean iteration time: ', num2str(mean(iteration)), ' [s]']);
display(['Max iteration time: ', num2str(max(iteration)), ' [s]']);
display(['Min iteration time: ', num2str(min(iteration)), ' [s]']);
bh_1e4 = loading/(100*mean(iteration));
display(['Proportion of serial time on 100 iterations: ', num2str(bh_1e4)]);
display(['Proportion of serial time on 1000 iterations: ', num2str(loading/(1000*mean(iteration)))]);
display('\n');

filename = '../results/bh_1e5_time.dat';
[total, simulation, loading, iteration, building, communication] = time(filename, algo, parallel);

display('Barnes-Hut - 1e5 bodies');

display(['Total time: ', num2str(total), ' [s]']);
display(['Loading time: ', num2str(loading), ' [s]']);
display(['Mean iteration time: ', num2str(mean(iteration)), ' [s]']);
display(['Max iteration time: ', num2str(max(iteration)), ' [s]']);
display(['Min iteration time: ', num2str(min(iteration)), ' [s]']);
bh_1e5 = loading/(100*mean(iteration));
display(['Proportion of serial time on 100 iterations: ', num2str(bh_1e5)]);
display(['Proportion of serial time on 1000 iterations: ', num2str(loading/(1000*mean(iteration)))]);
display('\n');

filename = '../results/bh_1e6_time.dat';
[total, simulation, loading, iteration, building, communication] = time(filename, algo, parallel);

display('Barnes-Hut - 1e6 bodies');

display(['Total time: ', num2str(total), ' [s]']);
display(['Loading time: ', num2str(loading), ' [s]']);
display(['Mean iteration time: ', num2str(mean(iteration)), ' [s]']);
display(['Max iteration time: ', num2str(max(iteration)), ' [s]']);
display(['Min iteration time: ', num2str(min(iteration)), ' [s]']);
bh_1e6 = loading/(100*mean(iteration));
display(['Proportion of serial time on 100 iterations: ', num2str(bh_1e6)]);
display(['Proportion of serial time on 1000 iterations: ', num2str(loading/(1000*mean(iteration)))]);
display('\n');

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
    