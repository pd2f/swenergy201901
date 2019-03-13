library(knitr)
library(readxl)
library(plotly)
library(dplyr)
library(gridExtra)
library(ggplot2)
library(scales)
library(ggrepel)
library(plotrix)


#consulta com eliminação dos registros de cabeçalho.
lista_de_participantes<- read_excel("Lista de participantes - Techstars_Startup_Weekend_So_Paulo_Energy (0128).xlsx",range ="A8:AA1000" )
# Eliminação dos registros de rodapé
participantes_dados <- lista_de_participantes[!is.na(lista_de_participantes$`Nº ingresso`),] %>%filter(`Estado de pagamento` == 'Aprovado')

#Dados participantes
summary(participantes_dados)
str(participantes_dados)
head(participantes_dados)

#características dos Participantes
caracteristicas_participantes <- select(participantes_dados,Gênero,Formação,`Data de nascimento`,`Tamanho de Camiseta`,`Tipo de ingresso`)
table(caracteristicas_participantes$Gênero,caracteristicas_participantes$`Tamanho de Camiseta`)


escolaridade <- as.data.frame(table(coalesce(caracteristicas_participantes$Formação,"Não Informada")))
names(escolaridade) <- c("Respostas","Frequencia")

coloursShot <- c("Pós-Graduação"="orange", "Ensino Médio Completo"="red", "Ensino Superior Completo"="green", "Ensino Superior Incompleto"="yellow","Não Informada"="grey")

escolaridade_decl <- function(resp){
  escolaridade %>% 
    count(Respostas) %>%
    ggplot(aes(x=reorder(Respostas, escolaridade$Frequencia), y=escolaridade$Frequencia/sum(escolaridade$Frequencia), fill=factor(Respostas))) +
    geom_bar(stat='identity', width = 0.7) + scale_fill_manual(values = coloursShot) +
    theme(legend.position="none") + coord_flip(ylim = c(0, 0.6)) + labs(x="", y=resp) +    
    scale_y_continuous(labels=percent)
}

pg <- escolaridade_decl('Escolaridade')

pg


#idade
install.packages("lubridate")
library(lubridate)
data_nas <- dmy(caracteristicas_participantes$`Data de nascimento`)
idades <- floor((today()-data_nas)/dyears(1))
summary(idades)
hist(idades)

#round((today()-data_nas)/dyears(1))
#ceiling((today()-data_nas)/dyears(1))
#as.period()
#print(caracteristicas_participantes[17,])

#emc <- escolaridade_decl('Ensino Médio Completo')
#esc <- escolaridade_decl('Ensino Superior Completo')
#esi <- escolaridade_decl('Ensino Superior Incompleto')

grid.arrange(pg, emc, esc, esi, ncol=1)



#Limpando variável Interesse
limpa_string <- gsub('\"',"",participantes_dados$`O que você espera encontrar na Techstars Startup Weekend?`)
#computando e convertendo variável interesse
interesse <- as.matrix(strsplit(limpa_string,","))
matriz_interesse <- matrix(unlist(interesse))
df_interesse = as.data.frame(table(na.omit(matriz_interesse)))
names(df_interesse) <- c("Respostas","Frequencia")

#Gerando gráfico de Interesse

ggplotly(df_interesse %>% 
           ggplot(aes(x=Respostas, y=Frequencia)) + geom_bar(stat='identity', fill='red') +
           labs(x='Resposta', y='Frequência', title='Interesse Declarado dos Participantes no SW ENERGY'))



##################################

pie3D(escolaridade$Frequencia,labels = escolaridade$Respostas,radius=0.95,explode=0.1,main="Percentual de Escolaridade Declarada")


ggplotly(escolaridade %>% 
           ggplot(aes(x=Respostas, y=Frequencia)) + geom_bar(stat='identity', fill='red') +
           labs(x='Resposta', y='Frequência', title='Percentual de Escolaridade Declarada'))


#Extraindo interessoe
library(ggplot2)



bp<- ggplot(df_interesse, aes(x="", y=Freq, fill=Var1))+
  geom_bar(width = 1, stat = "identity")

bp
plot(table(na.omit(matriz_interesse)),type = "pie")

ggplotly(PlantGrowth, aes(x=factor(1), fill=group))+
  geom_bar(width = 1)+
  coord_polar("y")


limpa_string <- gsub('"',"",limpa_string)


c(1,2,3,4,5)

interesse[54,1]

t <- as.vector(interesse)
summary(lista_de_participantes$interesse[49])


install.packages("plotrix",dependencies = TRUE)




