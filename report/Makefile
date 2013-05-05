
all: hams
	pdflatex report
	bibtex report
	pdflatex report
	pdflatex report

hams:
	pdflatex hams
	pdflatex hams
	pdflatex hams

clean:
	rm -f report.pdf hams.pdf *.log *.aux *.bbl *.blg

.PHONY: all
