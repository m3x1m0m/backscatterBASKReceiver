%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Author:       S3xm3x
% Date:         28.03.2017
% File:         save_samples.m
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

function saveSamples(lon,lat)

        %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
        % Vars
        %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%	
		whereSampsAre = '../Debug_Linux/'
		whereSampsShBe= '../samples/'	

        %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
        % Copying
        %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
		sampFile = strftime ('%Y_%m_%d_%H_%M_%S', localtime (time ()));
			if exist(['samples_' sampFile]', 'dir') ~= 7;
			system(['mkdir ' whereSampsShBe 'samples_' sampFile] );
		end
		system(['cp ' whereSampsAre 'raw.csv ' whereSampsShBe 'samples_' sampFile]);
		system(['cp ' whereSampsAre 'filtered.csv ' whereSampsShBe 'samples_' sampFile]);
		system(['cp ' whereSampsAre 'binary.csv ' whereSampsShBe 'samples_' sampFile]);
		disp('Copying done!');	
	
	%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
        % Save configuration data
        %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
		dlmwrite([whereSampsShBe 'samples_' sampFile '/conf.csv'],[lon lat]);	
endfunction;
