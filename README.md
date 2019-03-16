# POO [![Build Status](https://dev.azure.com/krosf/krosf/_apis/build/status/krosf-university.POO?branchName=P4)](https://dev.azure.com/krosf/krosf/_build/latest?definitionId=14&branchName=P4)

# Run

* install docker
* clone this repo
* `cd POO`
* `docker run -t --rm -v ${PWD}:/root/POO krosf/poo sh -c "cd /root/POO/P4 && make tests && make check && make valgrind"`
