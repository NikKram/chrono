@echo on
echo Process begins
call C:\Users\buildbot\Miniconda3\Scripts\activate.bat
call conda install --yes conda-build anaconda-client
call conda install --yes -c intel mkl-devel=2018.0.3
call conda build C:\codes\Chrono\Chrono_Source\contrib\packaging-python\conda --python=3.6 --no-remove-work-dir --dirty
call anaconda --token "%ANACONDA_TOKEN%" upload C:\Users\buildbot\Miniconda3\envs\base\win-64\pychrono-4.0.0*.bz2
echo "End Reached"