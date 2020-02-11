@echo on
echo Process begins
call "%CONDA_INSTALL_LOCN%"\Scripts\activate.bat
call conda install --yes conda-build anaconda-client
call conda install --yes -c intel mkl-devel=2018.0.3
call conda build .\contrib\packaging-python\conda --python=3.6 --no-remove-work-dir --dirty
call anaconda --token "%ANACONDA_TOKEN%" upload "%CONDA_INSTALL_LOCN%"\conda-bld\pychrono-4.0.0*.bz2
echo "End Reached"