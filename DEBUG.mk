# This makefile is intended to debug makefile's variables.

%:
	@echo '$*=$($*)'

d-%:
	@echo '$*=$($*)'
	@echo '    origin = $(origin $*)'
	@echo '    flavor = $(flavor $*)'
	@echo '    value = $(value $*)'

