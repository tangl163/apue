# This file is to debug various variables in makefile

%:
	@echo '$*=$($*)'

d-%:
	@echo '$*=$($*)'
	@echo '    origin = $(origin $*)'
	@echo '    flavor = $(flavor $*)'
	@echo '    value = $(value $*)'

