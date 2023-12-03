# M3_STR

<b>Disciplina:</b> Sistemas em Tempo Real.<br>
<b>Professor:</b> Felipe Viel. <br>
<b>Grupo:</b>André Valim, Eduardo Caldeira, Nicole Magagnin.<br><br>
<b>Enunciado:</b>
  O Trabalho da M3 consistirá em 2 partes que irão um único relatório.<br>
A primeira parte é a execução dos código das listas de exercícios de FreeRTOS e ESP-IDF, no caso a Lista RTOS (Exercícios 1,2,3,4,5 e 7) e Lista 2 RTOS. Deverão ser executados<br> os experimentos com os códigos e feito uma análise de cada funcionalidade expressada em cada código - descrever a funcionalidade e análise de execução dentro do <br>contexto de Sistema Operacional e Sistema Operacional de Tempo Real.
<br><br>
A segunda parte diz respeito ao desenvolvimento do código relativo ao Exercício - Threads (adaptação do código do primeiro trabalho para um contexto de código que seja<br> executável no FreeRTOS). Reaproveite o enunciando do trabalho feito na M1.<br>
Os códigos com Pthreads está disponível na pasta Exercícios (Exercício - Threads). Você deve emular a existência do problema (interrupção causado sobre identificação de pressão<br> elevada ou problema) usando o touch sensor com interrupção.<br>
Poderá ser feito para um sensor em um duto gás, um sensor em um duto de petróleo e um sensor em um poço de petróleo. No caso, há uma sensor que identifica ambas as pressões no<br> duto de gás e petróleo. Há também uma tarefa supervisora que é responsável por fazer o log de eventos (printar) na tela periodicamente. Os tempos de período de execução e apresentação pode ser os mesmos do trabalho da M1.<br>
A captura do tempo de execução pode ser feita usando as bibliotecas da ESP32 (link).<br>
Você poderá simular a comunicação de dados (via delay) ou usar algum sistema de comunicação de dados via Wi-Fi/Bluetooth. No caso da última opção, você terá a bonificação de até 2,0 pontos em uma das avaliações da M1 ou M2 (atribuição total ou não fica a critério do professor) ou 1 ponta na Média com a menor nota.<br>
Deverá ser feita uma apresentação de no máximo 5 minutos demonstrando essa solução.<br>
<br><br>
Informações do Trabalho:<br>
<ul>
<il>Poderá ser feito em até 3 pessoas.</il><br>
<il>O relatório deverá seguir o padrão IEEE (disponibilizado nas outras Ms).</il><br>
<il>O Relatório deverá ser entregue em formato PDF.</il><br>
<il>Os códigos devem estar em um repositório, o qual deverá ficar aberto após a entrega.</il><br>
<il>O vídeo de apresentação deverá ser postado preferencialmente no Youtube (como não listado). A não apresentação implicará em nota máxima não superior a 7,0.</il><br>
<il>Não será aceito trabalho entregue em atraso. Data máxima da entrega: 01/12/2021 às 23:59.</il><br>
<il>Deve ser usado o ESP-IDF como ambiente de desenvolvimento. Não será aceito via Arduino IDE.</il><br><br>
</ul>
