%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Author:       S3xm3x
% Date:         16.03.2017
% File:         response.m
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

	%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
        % Vars
        %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
		
		fileRaw = '../raw.csv'
		fileFiltered = '../filtered.csv'		

	%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
        % Readout and plot
        %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
			
		r=dlmread(fileRaw);
		f=dlmread(fileFiltered);		
		subplot(2,1,1);
		plot(r);
		xlabel('n');
		ylabel('Amplitude');
		title('Raw Data');
		subplot(2,1,2);
		plot(f);
		xlabel('n');
		ylabel('Amplitude');
		title('Filter Response');
