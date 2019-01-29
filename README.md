# POO [![Build Status](https://dev.azure.com/rodrigosanabria22/krosf/_apis/build/status/KROSF.POO?branchName=P4)](https://dev.azure.com/rodrigosanabria22/krosf/_build/latest?definitionId=11?branchName=P4)

# Run

* install docker
* clone this repo
* `cd POO`
* `docker run -t --rm -v ${PWD}:/root/POO krosf/poo sh -c "cd /root/POO/P4 && make tests && make check && make valgrind"`