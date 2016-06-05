function [total, simulation, loading, iteration, building, communication, assigning] = time(filename, algo, parallel)
% Function that processes the times in a file. And send them back
%
% INPUT:
%   - filename [String]: Name of the file created after the simulation
%   - algo [String]: Can either be 'barnes-hut' or 'brute-force'
%   - parallel [Bool]: True if a parallel version was used

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
        assigning = 0;

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
        assigning = 0;

    end
else
    if strcmp(algo,'brute-force')
        total = data(numel(data));
        simulation = data(numel(data)-1);
        loading = data(1);
        communication(1) = data(2);

        k=1;
        for i=3:2:numel(data)-5
            iteration(k) = data(i);
            communication(k+1) = data(i+1);
            k = k+1;
        end

        building = 0;
        assigning = 0;
    elseif strcmp(algo,'barnes-hut')
        total = data(numel(data));
        simulation = data(numel(data)-1);
        loading = data(1);
        communication(1) = data(2);
        building(1) = data(3);

        k=1;
        for i=4:4:numel(data)-5
            assigning(k) = data(i);
            communication(k+1) = data(i+1);
            building(k+1) = data(i+2);
            iteration(k) = data(i+3);
            k = k+1;
        end
    end
end

end
