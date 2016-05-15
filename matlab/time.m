function [total, simulation, loading, iteration, building, communication] = time(filename, algo, parallel)
% Function that processes the times. And send them back

delimiterIn = ',';
headerlinesIn = 1;
data = importdata(filename,delimiterIn,headerlinesIn);
data = data.data;

if parallel == false
    if strcmp(algo,'brute-force')
        total = data(numel(data));
        simulation = data(numel(data)-1);
        loading = data(1);
        
        k=1;
        for i=2:1:numel(data)-2
            iteration(k) = data(i);
            k = k+1;
        end 
        
        building = 0;
        communication = 0;
        
    elseif strcmp(algo,'barnes-hut')
        total = data(numel(data));
        simulation = data(numel(data)-1);
        loading = data(1);
        building(1) = data(2);
        
        k=1;
        for i=3:2:numel(data)-5
            iteration(k) = data(i);
            building(k+1) = data(i+1);
            k = k+1;
        end  
        
        communication = 0;
        
    end
else
    if strcmp(algo,'brute-force')
        total = data(numel(data));
        simulation = 0;
        loading = 0;
        iteration = 0;
        building = 0;
        communication = 0;
    elseif strcmp(algo,'barnes-hut')
        total = data(numel(data));
        simulation = 0;
        loading = 0;
        iteration = 0;
        building = 0;
        communication = 0;        
    end
end

end

