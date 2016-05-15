%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                                         %
%Process the times of the Algo to see if their theoretical time is correct%
%                                                                         %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

clear all;
close all;
clc;

figure();

% First, we loglog the values for Brute-Force
nbr = [1e1, 2e1, 5e1, 1e2, 2e2, 5e2, 1e3, 2e3, 5e3, 1e4, 2e4, 5e4, 1e5]
nbrStr = {'1e1', '2e1', '5e1', '1e2', '2e2', '5e2', '1e3', '2e3', '5e3', '1e4', '2e4', '5e4', '1e5'};

algo = 'brute-force';
parallel = false;
for i=1:length(nbr)
   
    filename = ['../results/bf_', nbrStr{i}, '_time.dat'];
    
    [total, simulation, loading, iteration, building, communication] = time(filename, algo, parallel);
    
    bf_time(i) = mean(iteration);    
end

loglog(nbr, bf_time, 'k', 'LineWidth', 1.1);
hold on;

algo = 'barnes-hut';

for i=1:length(nbr)-1
   
    filename = ['../results/bh_', nbrStr{i}, '_0_time.dat'];
    
    [total, simulation, loading, iteration, building, communication] = time(filename, algo, parallel);
    
    bh_0_time(i) = mean(iteration(2:end));    
end

loglog(nbr(1:end-1), bh_0_time, 'b', 'LineWidth', 1.1);
hold on;

for i=1:length(nbr)
   
    filename = ['../results/bh_', nbrStr{i}, '_05_time.dat'];
    
    [total, simulation, loading, iteration, building, communication] = time(filename, algo, parallel);
    
    bh_05_time(i) = mean(iteration(2:end));    
end

loglog(nbr, bh_05_time, 'b--', 'LineWidth', 1.1);
hold on;

for i=1:length(nbr)
   
    filename = ['../results/bh_', nbrStr{i}, '_1_time.dat'];
    
    [total, simulation, loading, iteration, building, communication] = time(filename, algo, parallel);
    
    bh_1_time(i) = mean(iteration(2:end));    
end

loglog(nbr, bh_1_time, 'b:', 'LineWidth', 1.5);
hold on;

% for i=1:length(nbr)
%    
%     filename = ['../results/bh_', nbrStr{i}, '_2_time.dat'];
%     
%     [total, simulation, loading, iteration, building, communication] = time(filename, algo, parallel);
%     
%     bh_2_time(i) = mean(iteration(2:end));    
% end
% 
% loglog(nbr, bh_2_time);
% hold on;

for i=1:length(nbr)
   
    filename = ['../results/bh_', nbrStr{i}, '_5_time.dat'];
    
    [total, simulation, loading, iteration, building, communication] = time(filename, algo, parallel);
    
    bh_5_time(i) = mean(iteration(2:end));    
end

loglog(nbr, bh_5_time, 'b-.', 'LineWidth', 1.1);
hold on;

% for i=1:length(nbr)
%    
%     filename = ['../results/bh_', nbrStr{i}, '_10_time.dat'];
%     
%     [total, simulation, loading, iteration, building, communication] = time(filename, algo, parallel);
%     
%     bh_10_time(i) = mean(iteration(2:end));    
% end
% 
% loglog(nbr, bh_10_time);
% hold on;

loglog(nbr, 1e-7*nbr.^2, 'r', 'LineWidth', 1.1);
hold on;

loglog(nbr, 1e-7*nbr.*log(nbr), 'r--', 'LineWidth', 1.1);
hold on;

grid on;
xlabel('Number of bodies');
ylabel('Time [s]');
legend('Brute-Force', 'Barnes-Hut - \theta = 0', 'Barnes-Hut - \theta = 0.5', ...
    'Barnes-Hut - \theta = 1', 'Barnes-Hut - \theta = 5', 'O(n^2)',...
    'O(n log n)','Location', 'northwest');