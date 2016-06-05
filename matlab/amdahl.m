%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                                         %
%                                 Amdahl                                  %
%                                                                         %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

clc;
clear all;
close all;

filename = '../results-strong-scaling/bf_1e4_1_time.dat';
parallel = true;
algo = 'brute-force';

[total, simulation, loading, iteration, building, communication, assigning] = time(filename, algo, parallel);

display('Brute-Force - 1e4 bodies');

display(['Total time: ', num2str(total), ' [s]']);
display(['Loading time: ', num2str(loading), ' [s]']);
display(['Mean iteration time: ', num2str(mean(iteration)), ' [s]']);
display(['Mean communication time: ', num2str(mean(communication)), ' [s]']);
bf_1e4 = mean(communication)/mean(iteration);
display(['Proportion of serial time: ', num2str(bf_1e4)]);
display(' ');

filename = '../results-strong-scaling/bf_1e5_1_time.dat';
[total, simulation, loading, iteration, building, communication, assigning] = time(filename, algo, parallel);

display('Brute-Force - 1e5 bodies');

display(['Total time: ', num2str(total), ' [s]']);
display(['Loading time: ', num2str(loading), ' [s]']);
display(['Mean iteration time: ', num2str(mean(iteration)), ' [s]']);
display(['Mean communication time: ', num2str(mean(communication)), ' [s]']);
bf_1e5 = mean(communication)/mean(iteration);
display(['Proportion of serial time: ', num2str(bf_1e5)]);
display(' ');

filename = '../results-strong-scaling/bf_1e6_1_time.dat';
[total, simulation, loading, iteration, building, communication, assigning] = time(filename, algo, parallel);

display('Brute-Force - 1e6 bodies');

display(['Total time: ', num2str(total), ' [s]']);
display(['Loading time: ', num2str(loading), ' [s]']);
display(['Mean iteration time: ', num2str(mean(iteration)), ' [s]']);
display(['Mean communication time: ', num2str(mean(communication)), ' [s]']);
bf_1e6 = mean(communication)/mean(iteration);
display(['Proportion of serial time: ', num2str(bf_1e6)]);
display(' ');

filename = '../results-strong-scaling/bh_1e4_1_time.dat';
parallel = true;
algo = 'barnes-hut';

[total, simulation, loading, iteration, building, communication, assigning] = time(filename, algo, parallel);

display('Barnes-Hut - 1e4 bodies');

display(['Total time: ', num2str(total), ' [s]']);
display(['Loading time: ', num2str(loading), ' [s]']);
display(['Mean iteration time: ', num2str(mean(iteration)), ' [s]']);
display(['Mean communication time: ', num2str(mean(communication)), ' [s]']);
display(['Mean building time: ', num2str(mean(building)), ' [s]']);
display(['Mean assigning time: ', num2str(mean(assigning)), ' [s]']);
bh_1e4 = (mean(communication)+mean(building)+mean(assigning))/mean(iteration);
display(['Proportion of serial time: ', num2str(bh_1e4)]);
display(' ');

filename = '../results-strong-scaling/bh_1e5_1_time.dat';
[total, simulation, loading, iteration, building, communication, assigning] = time(filename, algo, parallel);

display('Barnes-Hut - 1e5 bodies');

display(['Total time: ', num2str(total), ' [s]']);
display(['Loading time: ', num2str(loading), ' [s]']);
display(['Mean iteration time: ', num2str(mean(iteration)), ' [s]']);
display(['Mean communication time: ', num2str(mean(communication)), ' [s]']);
display(['Mean building time: ', num2str(mean(building)), ' [s]']);
display(['Mean assigning time: ', num2str(mean(assigning)), ' [s]']);
bh_1e5 = (mean(communication)+mean(building)+mean(assigning))/mean(iteration);
display(['Proportion of serial time: ', num2str(bh_1e5)]);
display(' ');

filename = '../results-strong-scaling/bh_1e6_1_time.dat';
[total, simulation, loading, iteration, building, communication, assigning] = time(filename, algo, parallel);

display('Barnes-Hut - 1e6 bodies');

display(['Total time: ', num2str(total), ' [s]']);
display(['Loading time: ', num2str(loading), ' [s]']);
display(['Mean iteration time: ', num2str(mean(iteration)), ' [s]']);
display(['Mean communication time: ', num2str(mean(communication)), ' [s]']);
display(['Mean building time: ', num2str(mean(building)), ' [s]']);
display(['Mean assigning time: ', num2str(mean(assigning)), ' [s]']);
bh_1e6 = (mean(communication)+mean(building)+mean(assigning))/mean(iteration);
display(['Proportion of serial time: ', num2str(bh_1e6)]);
display(' ');

speedup = @(alpha, p) 1/(alpha + (1-alpha)/p);

k=1;
for i=1:50
    nodes(k) = i;
    sp_bf_1e4(k) = speedup(bf_1e4, i);
    sp_bf_1e5(k) = speedup(bf_1e5, i);
    sp_bf_1e6(k) = speedup(bf_1e6, i);
    sp_bh_1e4(k) = speedup(bh_1e4, i);
    sp_bh_1e5(k) = speedup(bh_1e5, i);
    sp_bh_1e6(k) = speedup(bh_1e6, i);
    k = k+1;
end

save('Amdahl', 'sp_bf_1e4', 'sp_bf_1e5', 'sp_bf_1e6', 'sp_bh_1e4', 'sp_bh_1e5', 'sp_bh_1e6', 'nodes');

figure()
loglog(nodes, sp_bf_1e4, 'k', 'LineWidth', 1.2);
hold on;
loglog(nodes, sp_bf_1e5, 'k--', 'LineWidth', 1.2);
loglog(nodes, sp_bf_1e6, 'k:', 'LineWidth', 1.5);
loglog(nodes, sp_bh_1e4, 'b', 'LineWidth', 1.2);
loglog(nodes, sp_bh_1e5, 'b--', 'LineWidth', 1.2);
loglog(nodes, sp_bh_1e6, 'b:', 'LineWidth', 1.5);
legend('Brute-force - 10^4 bodies', 'Brute-force - 10^5 bodies', 'Brute-force - 10^6 bodies', ...
    'Barnes-Hut - 10^4 bodies', 'Barnes-Hut - 10^5 bodies', 'Barnes-Hut - 10^6 bodies', 'Location', 'northwest')
xlabel('Number of nodes');
ylabel('Theoretical speedup');
set(gca,'XTick',[1, 2, 5, 10, 20, 50], 'XTickLabel',{'1', '2', '5', '10', '20', '50'})
set(gca,'YTick',[1, 2, 5, 10, 20, 50], 'YTickLabel',{'1', '2', '5', '10', '20', '50'})
axis([1 50 1 50]); 

grid on;
