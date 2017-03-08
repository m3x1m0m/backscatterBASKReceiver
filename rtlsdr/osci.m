%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Author:       S3xm3x
% Date:         08.03.2017
% File:         osci.m
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

	%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
        % Vars
        %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
		
		samp_rate = 1.8e6			% Hertz
		refresh_rate =	0.05			% Seconds 
		N = samp_rate*refresh_rate		% Number of samples
		file = 'raw.csv'
		n = 100				% Decimation factor	

	%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
        % Readout and plot
        %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
				
		offset = 0;
		C0 = 0;
		C1 = 0;
		t = linspace(0,refresh_rate*1e3,N/n);	
		M = zeros(1,N);
		P = M(1:n:end);		
		osci = plot(t,P);
		xlabel('t/ms');
		ylabel('8 Bit value');
		title('Max'' Scope');
		maxVal = 0;
		minVal = 0;
		grid on;

		while (length(M) >= N)
			R0 = offset * N;
			R1 = offset * N + N - 1;
			M = dlmread(file,',',[R0,C0,R1,C1]);
			P = M(1 : n : end);			% Decimate!
			if ( maxVal < (a = max(P)) )
				maxVal = a;
			end
			if ( minVal > (a = min(P)) )
				minVal = a;
			end				
			axis([0,refresh_rate*1e3,minVal*1.1,maxVal*1.1]);	
			set(osci, 'XData', t, 'YData', P); 
			offset = offset + 1;
			pause(refresh_rate);			
		end
