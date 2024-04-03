# Kit RTD

## Colocar simulador a funcionar:
  1. Colocar a pasta RTD/Simulador/Visual Studio/str/ no C: de modo a ficar C:/str/STR_RTD/...
  2. Abrir o STR_RTD.sln que está dentro da pasta C:/str/STR_RTD/
  3. Abrir a RTD Simulator.exe que está na pasta RTD/Simulador/Executável/Windows/
  4. Correr o Visual Studio
  5. No Visual Studio, escolher a opção pretendida (1, 2 ou 3)
  6. Na Unity, spawnar o bloco
------------------------------------------------------------------------------------------
## Colocar o kit fisico a funcionar:
  1. Por fazer
  2. Fazer
------------------------------------------------------------------------------------------
## Comunicação

### Visual Studio -> Unity
[Isto serve para trocar bits no Unity] \
Socket com String \
Ip da Unity -> localhost:3000
### Set Bit:
"SX V" \
Onde X é o número do port e V é o valor do bit (0 ou 1) 
### Get Bit:
"GX" \
Onde X é o número do port

------------------------------------------------------------------------------------------

### Posts
[Isto serve para enviar skills para o Unity] \
HTTP posts com key \
Ip da Unity -> localhost:80

### Exemplo de post
localhost:80/transporte/?skill=C1SL1

### Skills disponiveis
C1SL1 \
SL1C2 \
C2C3 \
C3SL2 \
SL2C4 \
mill \
drill

------------------------------------------------------------------------------------------

## Sensores IX

| Number | Function          | Active |
|--------|-------------------|--------|
| 1      | Slider 1 Front    | High   |
| 2      | Slider 1 Rear     | High   |
| 3      | Slider 2 Front    | High   |
| 4      | Slider 2 Rear     | High   |
| 5      | Conveyor Slider 1 | Low    |
| 6      | Conveyor Milling  | Low    |
| 7      | Conveyor Start    | Low    |
| 8      | Conveyor Drilling | Low    |
| 9      | Conveyor End      | Low    |

------------------------------------------------------------------------------------------

## Atuadores QX

| Number | Function          |
|--------|-------------------|
| 1      | Slider 1 Forward  |
| 2      | Slider 1 Backward |
| 3      | Slider 2 Forward  |
| 4      | Slider 2 Backward |
| 5      | Conveyor Start    |
| 6      | Conveyor Milling  |
| 7      | Milling Spin      |
| 8      | Conveyor Drilling |
| 9      | Drilling Spin     |
| 10     | Conveyor End      |
