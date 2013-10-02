# set latexfile to the name of the main file without the .tex
latexfile = plos_template
otherlatexfile = results.tex discussion.tex materialmethods.tex introduction.tex 
bibtexfile = references.bib
# put the names of figure files here.  include the .eps
#figures = AntarcticFoodWebScheme.eps ArcticFoodWebScheme.eps AverIndInTime_50Real_1000Ind.eps AverIndInTime_50Real_10Ind.eps Dist_Kind_Diet_AntarcticCleanFoodWeb.eps Dist_Kind_Diet_ArcticFoodWeb.eps Dist_Levels_AntarcticCleanFoodWeb.eps Dist_Levels_ArcticFoodWeb.eps FoodWebModel_Predation.eps FoodWeb_9species.eps FoodWeb_30bloques9.eps Legend_KindDiet.eps Pred_AntarcticCleanFoodWeb.eps Pred_ArcticFoodWeb.eps Prey_AntarcticCleanFoodWeb.eps Prey_ArcticFoodWeb.eps TrophicLevels_30species.eps
TEX = latex
BIB = bibtex

# *.fig files may be in ./Figs
vpath %.fig Figs

# reruns latex if needed.  to get rid of this capability, delete the
# three lines after the rule. 
# idea from http://ctan.unsw.edu.au/help/uk-tex-faq/Makefile
$(latexfile).dvi : $(latexfile).tex $(otherlatexfile) $(bibtexfile)
	$(TEX) $(latexfile) ; \
	$(BIB) $(latexfile) ; \
	$(TEX) $(latexfile) ; \
	$(TEX) $(latexfile) ; 

%.eps : %.fig
	fig2dev -L eps $< > $@

$(latexfile).pdf : $(latexfile).ps
	ps2pdf -dAutoRotatePages=/None $(latexfile).ps $(latexfile).pdf

pdf : $(latexfile).pdf

$(latexfile).ps : $(latexfile).dvi
	dvips $(latexfile)

ps : $(latexfile).ps 

clean:
	rm -Rf *.bbl *.blg *.aux *.log *.ps *.dvi $(latexfile).pdf *.out *.nav *.snm *.toc
view:
	xdvi $(latexfile).dvi& 
