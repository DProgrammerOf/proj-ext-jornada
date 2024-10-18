# Projeto Jornada de Trabalho

 Objetivo: Desenvolver um dispositivo que se comunique com o rastreador **GS10G ONE-WIRE** (WanWay Tech) a fim de parametrizar as jornadas de trabalho dos motoristas com a utilização de uma interface simples de interação com dois botões de ação individual.

## Componentes utilizados: 
* Arduino Nano v3
* Placa de terminais adaptadas com bornes
* 2x Botões de 12mm
* 2x LED
* Regulador de tensão LM317

## Tecnologias utilizadas:
* C / C++
* Java
* MariaDB

## Metodologia: 
1. O dispositivo do projeto foi interligado por 3 fios: **DATA**, **GND** para comunicação e **VDD** para alimentação.
2. A comunicação entre o dipositivo e o rastreador **GS10G** foi realizada usando o protocolo [1-Wire](https://pt.wikipedia.org/wiki/One_wire).
3. O **firmware** monitora os estados dos botões, os _bits_ escritos pelo _master_ no barramento e o gerenciamento da fila de ações em [FIFO](https://pt.wikipedia.org/wiki/FIFO).
5. Cada botão está vinculado a uma ação, sendo um _serial_ de _8 bytes_, no qual é enviado ao _master_ pelo barramento:
   - **Inicio de Jornada**
     - `0x01, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00`
   - **Fim de Jornada**
     - `0x01, 0x01, 0x02, 0x00, 0x00, 0x00, 0x00`
   - **Hora de Almoço**
     - `0x01, 0x01, 0x03, 0x00, 0x00, 0x00, 0x00`
6. O pacote no qual é associado o _serial_ é denominado [Information Transfer Packet](https://github.com/DProgrammerOf/proj-ext-jornada/blob/main/InformationTransferPacket.png), descrito na documentação do fornecedor [GT06](https://github.com/DProgrammerOf/proj-ext-jornada/blob/main/JM-VL03%20GPS%20Tracker%20Communication%20Protocol_v1.1_20210207%202.pdf).
7. O tratamento das informações é feito pelo software [Traccar 4.11](https://github.com/traccar/traccar/tree/v4.11) com customizações para funcionar com esse projeto: [Gt06ProtocolDecoder](https://github.com/DProgrammerOf/proj-ext-jornada/blob/main/software-custom/Gt06ProtocolDecoder.java).

## Fluxo de uso:
1. O motorista aperta o botão de ação assim que ligada a ignição do veículo.
2. A interface do dispositivo possuí 2 botões com um texto abaixo e fornece uma resposta visual da ação feita:
   - Viagem
     - O início de jornada é representado pelo `LED AZUL` piscar `1 vez`.
     - O fim de jornada é representado pelo `LED AZUL` piscar `3 vezes`.
   - Almoço
     - A hora de almoço é representado pelo `LED AZUL` piscar `2 vezes`.
3. Uma notificação é enviada para o dono do veículo.

## Testes:
### Protótipo
![](https://github.com/DProgrammerOf/proj-ext-jornada/blob/main/teste-2-video.gif)
### Final
![](https://github.com/DProgrammerOf/proj-ext-jornada/blob/main/teste-final-video.gif)
