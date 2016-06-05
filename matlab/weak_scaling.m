clear all;
close all;
clc;

algo = 'barnes-hut';
parallel = true;

process = [1, 2, 3, 4, 5, 6, 7, 8];

file = '../results-weak-scaling/bh';

runningTime_bh_e4 = zeros(length(process),1);

for i=1:length(process)
    
    filename = [file,'_',num2str(process(i)),'e4_', num2str(process(i)), '_time.dat'];
    
    [~, ~, ~, iteration, ~, communication, ~] = time(filename, algo, parallel);
    
    runningTime_bh_e4(i) = mean(iteration) - mean(communication);
    
end

weakScaling_bh_e4 = zeros(length(process),1);

weakScaling_bh_e4(:) = runningTime_bh_e4(1)./runningTime_bh_e4(:);

runningTime_bh_e5 = zeros(length(process),1);

for i=1:length(process)
    
    filename = [file,'_',num2str(process(i)),'e5_', num2str(process(i)), '_time.dat'];
    
    [~, ~, ~, iteration, ~, communication, ~] = time(filename, algo, parallel);
    
    runningTime_bh_e5(i) = mean(iteration) - mean(communication);
    
end

weakScaling_bh_e5 = zeros(length(process),1);

weakScaling_bh_e5(:) = runningTime_bh_e5(1)./runningTime_bh_e5(:);

algo = 'brute-force';

file = '../results-weak-scaling/bf';

runningTime_bf_e4 = zeros(length(process),1);

for i=1:length(process)
    
    filename = [file,'_',num2str(process(i)),'e4_', num2str(process(i)), '_time.dat'];
    
    [~, ~, ~, iteration, ~, communication, ~] = time(filename, algo, parallel);
    
    runningTime_bf_e4(i) = mean(iteration) - mean(communication);
    
end

weakScaling_bf_e4 = zeros(length(process),1);

weakScaling_bf_e4(:) = runningTime_bf_e4(1)./runningTime_bf_e4(:);

runningTime_bf_e5 = zeros(length(process),1);

for i=1:length(process)
    
    filename = [file,'_',num2str(process(i)),'e5_', num2str(process(i)), '_time.dat'];
    
    [~, ~, ~, iteration, ~, communication, ~] = time(filename, algo, parallel);
    
    runningTime_bf_e5(i) = mean(iteration) - mean(communication);
    
end

weakScaling_bf_e5 = zeros(length(process),1);

weakScaling_bf_e5(:) = runningTime_bf_e5(1)./runningTime_bf_e5(:);

figure()
subplot(2,1,1)
loglog(process, runningTime_bf_e4, 'b', 'LineWidth', 1.2)
hold on;
loglog(process, runningTime_bf_e5, 'b--', 'LineWidth', 1.2)
ylabel('Running time [s]');
xlabel('Number of nodes');
%axis([1 8 0.01 10])
%set(gca,'YTick',[0.02, 0.05, 0.1, 0.2, 0.5, 1, 2, 5], 'YTickLabel',{'0.02', '0.05', '0.1', '0.2', '0.5', '1', '2', '5'})
%set(gca,'XTick',[1, 2, 3, 4, 5, 6, 7, 8], 'XTickLabel',{'1', '2', '3', '4', '5', '6', '7', '8'})
grid on;
subplot(2,1,2)
semilogx(process, weakScaling_bf_e4, 'b', 'LineWidth', 1.2)
hold on;
semilogx(process, weakScaling_bf_e5, 'b--', 'LineWidth', 1.2)
ylabel('Efficiency');
xlabel('Number of nodes');
%set(gca,'YTick',[0, 0.2, 0.4, 0.6, 0.8, 1.0], 'YTickLabel',{'0', '0.2', '0.4', '0.6', '0.8', '1'})
%set(gca,'XTick',[1, 2, 3, 4, 5, 6, 7, 8], 'XTickLabel',{'1', '2', '3', '4', '5', '6', '7', '8'})
legend('Brute-Force - 10^4 bodies per node','Brute-Force - 10^5 bodies per node');
grid on;

figure()
subplot(2,1,1)
loglog(process, runningTime_bh_e4, 'b', 'LineWidth', 1.2)
hold on;
loglog(process, runningTime_bh_e5, 'b--', 'LineWidth', 1.2)
ylabel('Running time [s]');
xlabel('Number of nodes');
%axis([1 8 0.01 10])
%set(gca,'YTick',[0.02, 0.05, 0.1, 0.2, 0.5, 1, 2, 5], 'YTickLabel',{'0.02', '0.05', '0.1', '0.2', '0.5', '1', '2', '5'})
%set(gca,'XTick',[1, 2, 3, 4, 5, 6, 7, 8], 'XTickLabel',{'1', '2', '3', '4', '5', '6', '7', '8'})
grid on;

subplot(2,1,2)
semilogx(process, weakScaling_bh_e4, 'b', 'LineWidth', 1.2)
hold on;
semilogx(process, weakScaling_bh_e5, 'b--', 'LineWidth', 1.2)
ylabel('Efficiency');
xlabel('Number of nodes');
%set(gca,'YTick',[0, 0.2, 0.4, 0.6, 0.8, 1.0], 'YTickLabel',{'0', '0.2', '0.4', '0.6', '0.8', '1'})
%set(gca,'XTick',[1, 2, 3, 4, 5, 6, 7, 8], 'XTickLabel',{'1', '2', '3', '4', '5', '6', '7', '8'})
legend('Barnes-Hut - 10^4 bodies per node','Barnes-Hut - 10^5 bodies per node');
grid on;

