clear all;
close all;
clc;

algo = 'brute-force';
parallel = true;

process = [1, 2, 4, 6, 8, 10, 14, 18, 24, 30, 36, 42, 48];

file = '../results-strong-scaling/bf_1e4';

speedup_bf_1e4 = zeros(length(process),1);

for i=1:length(process)
    
    filename = [file,'_',num2str(process(i)),'_time.dat'];
    
    [~, ~, ~, iteration, ~, communication, ~] = time(filename, algo, parallel);
    
    speedup_bf_1e4(i) = mean(iteration) - mean(communication);
    
end

speedup_bf_1e4(:) = speedup_bf_1e4(1)./speedup_bf_1e4(:);

file = '../results-strong-scaling/bf_1e5';

speedup_bf_1e5 = zeros(length(process),1);

for i=1:length(process)
    
    filename = [file,'_',num2str(process(i)),'_time.dat'];
    
    [~, ~, ~, iteration, ~, communication, ~] = time(filename, algo, parallel);
    
    speedup_bf_1e5(i) = mean(iteration) - mean(communication);
    
end

speedup_bf_1e5(:) = speedup_bf_1e5(1)./speedup_bf_1e5(:);

file = '../results-strong-scaling/bf_1e6';

speedup_bf_1e6 = zeros(length(process),1);

for i=1:length(process)
    
    filename = [file,'_',num2str(process(i)),'_time.dat'];
    
    [~, ~, ~, iteration, ~, communication, ~] = time(filename, algo, parallel);
    
    speedup_bf_1e6(i) = mean(iteration) - mean(communication);
    
end

speedup_bf_1e6(:) = speedup_bf_1e6(1)./speedup_bf_1e6(:);

algo = 'barnes-hut';

file = '../results-strong-scaling/bh_1e4';

speedup_bh_1e4 = zeros(length(process),1);

for i=1:length(process)
    
    filename = [file,'_',num2str(process(i)),'_time.dat'];
    
    [~, ~, ~, iteration, ~, communication, ~] = time(filename, algo, parallel);
    
    speedup_bh_1e4(i) = mean(iteration) - mean(communication);
    
end

speedup_bh_1e4(:) = speedup_bh_1e4(1)./speedup_bh_1e4(:);

file = '../results-strong-scaling/bh_1e5';

speedup_bh_1e5 = zeros(length(process),1);

for i=1:length(process)
    
    filename = [file,'_',num2str(process(i)),'_time.dat'];
    
    [~, ~, ~, iteration, ~, communication, ~] = time(filename, algo, parallel);
    
    speedup_bh_1e5(i) = mean(iteration) - mean(communication);
    
end

speedup_bh_1e5(:) = speedup_bh_1e5(1)./speedup_bh_1e5(:);

file = '../results-strong-scaling/bh_1e6';

speedup_bh_1e6 = zeros(length(process),1);

for i=1:length(process)
    
    filename = [file,'_',num2str(process(i)),'_time.dat'];
    
    [~, ~, ~, iteration, ~, communication, ~] = time(filename, algo, parallel);
    
    speedup_bh_1e6(i) = mean(iteration) - mean(communication);
    
end

speedup_bh_1e6(:) = speedup_bh_1e6(1)./speedup_bh_1e6(:);

load('Amdahl');


figure()
loglog(nodes, sp_bf_1e4, 'k', 'LineWidth', 1.2);
hold on;
%loglog(nodes, sp_bf_1e5, 'k--', 'LineWidth', 1.2);
%loglog(nodes, sp_bf_1e6, 'k:', 'LineWidth', 1.5);
loglog(process, speedup_bf_1e4, 'b', 'LineWidth', 1.2);
loglog(process, speedup_bf_1e5, 'b--', 'LineWidth', 1.2);
loglog(process, speedup_bf_1e6, 'b:', 'LineWidth', 1.5);
%legend('Theoretical - 10^4 bodies', 'Theoretical - 10^5 bodies', 'Theoretical - 10^6 bodies', ...
%    'Experience - 10^4 bodies', 'Experience - 10^5 bodies', 'Experience - 10^6 bodies', 'Location', 'southeast')
legend('Theoretical', 'Experience - 10^4 bodies', 'Experience - 10^5 bodies', 'Experience - 10^6 bodies', 'Location', 'southeast')
xlabel('Number of nodes');
ylabel('Theoretical speedup');
grid on;
set(gca,'XTick',[1, 2, 5, 10, 20, 50], 'XTickLabel',{'1', '2', '5', '10', '20', '50'})
set(gca,'YTick',[1, 2, 5, 10, 20, 50], 'YTickLabel',{'1', '2', '5', '10', '20', '50'})
axis([1 50 1 50]); 


figure()
loglog(nodes, sp_bh_1e4, 'k', 'LineWidth', 1.2);
hold on;
loglog(nodes, sp_bh_1e5, 'k--', 'LineWidth', 1.2);
loglog(nodes, sp_bh_1e6, 'k:', 'LineWidth', 1.5);
loglog(process, speedup_bh_1e4, 'b', 'LineWidth', 1.2);
loglog(process, speedup_bh_1e5, 'b--', 'LineWidth', 1.2);
loglog(process, speedup_bh_1e6, 'b:', 'LineWidth', 1.5);
legend('Theoretical - 10^4 bodies', 'Theoretical - 10^5 bodies', 'Theoretical - 10^6 bodies', ...
    'Experience - 10^4 bodies', 'Experience - 10^5 bodies', 'Experience - 10^6 bodies', 'Location', 'southeast')
xlabel('Number of nodes');
ylabel('Theoretical speedup');
grid on;
set(gca,'XTick',[1, 2, 5, 10, 20, 50], 'XTickLabel',{'1', '2', '5', '10', '20', '50'})
set(gca,'YTick',[1, 2, 3, 4, 5, 6], 'YTickLabel',{'1', '2', '3', '4', '5', '6'})
axis([1 50 1 6]); 



