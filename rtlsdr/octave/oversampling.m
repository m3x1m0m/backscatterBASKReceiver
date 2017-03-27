%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Author:       S3xm3x
% Date:         27.03.2017
% File:         oversampling.m
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

		clear;
        %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
        % Vars
        %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
		overSampFact = 40;
		inputData = '../samples/sim_test_1.csv';
		outputData = '../samples/sim_test_2.csv';
		filterFile = '../filters/oversampling_2.5e6_10e3.csv';

	%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
        % Oversampling
        %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%		
		x= [1 ; zeros(99, 1)];		
		R = dlmread(inputData);
		Rimag = R(:,2);
		Rreal = R(:,1);
		Rimagov = interp1( linspace(1,length(Rimag),length(Rimag)),Rimag,linspace(1,length(Rimag),length(Rimag)*overSampFact),'spline' );
		Rrealov = interp1( linspace(1,length(Rreal),length(Rreal)),Rreal,linspace(1,length(Rreal),length(Rreal)*overSampFact),'spline' );
		b = dlmread(filterFile);
		Rimag=filter(b,1,Rimagov);
		Rreal=filter(b,1,Rrealov);
		Rimag = Rimag(1:overSampFact:end);
		Rreal = Rreal(1:overSampFact:end);

	%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
        % Save result
        %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%	
		
		R(:,1) = Rreal;
		R(:,2) = Rimag;	
		dlmwrite(outputData,R);	
