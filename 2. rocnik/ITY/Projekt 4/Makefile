NAME=proj4

all: $(NAME).tex
	latex $(NAME).tex
	bibtex $(NAME).aux
	latex $(NAME).tex
	latex $(NAME).tex
	dvips -t a4 $(NAME).dvi
	ps2pdf $(NAME).ps

clean:
	rm $(NAME).log $(NAME).pdf $(NAME).aux $(NAME).ps $(NAME).dvi $(NAME).blg $(NAME).bbl