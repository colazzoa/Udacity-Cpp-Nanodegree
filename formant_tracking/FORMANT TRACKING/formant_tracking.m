clear all;
clc;
close all;


V = ['a_'; 'ea'; 'ec' ;'i_';'oa';'oc';'u_']
for t=1:7


    
[x1, Fs]=wavread(V(t,:)); %lettura segnale audio

pre=1; %se 1 pre-enfasi, se 0 no
if(pre)
x=filter([1, -0.98], 1, x1); %pre-enfasi
else
    x=x1; % no pre-enfasi
end

nwin=320; %dimensione finestra 

p=10; %ordine filtro predizione
w=hamming(nwin); %finestra di hamming di dimensione nwin
n=1;
j=1;
ninc=8; %spostamento in avanti della finestra

while(n+nwin <= length(x))
    xfin=x(n:n+nwin-1).*w;
    [a, EQM] = lpc(xfin,p); %calcolo parametri LP per ogni frame di segnale
    
    % Calcolo Formanti
roots_a=roots(a);
roots_a(find(imag(roots_a(:,1))==0))=NaN; % cfr relazione
roots_a(find(abs(roots_a(:,1))<0.8))=NaN; % cfr relazione
roots_a(find(abs(roots_a(:,1))>1))=NaN;   % cfr relazione

 %NB: plot ignora NaN
 
formants_a=angle(roots_a)/(2*pi)*8000; %frequenza della formante

formants_a=formants_a'; %da colonna a riga
a_sorted(j,:) = sort(abs(formants_a)); %ordino le formanti 
j=j+1;
n=n+ninc;
end


figure(t)

subplot(3,1,1)
plot((1:length(x1)).*(Fs^-1),x1) %ampiezza del segnale nel tempo
xlabel('Tempo [s]')
ylabel('x(t)')
title( V(t,:)) 
grid on
subplot(3,1,2)
Nfft= 512;
L= 320;	
R= 8;
Noverlap= L-R;
spectrogram(x1,L,Noverlap,Nfft,Fs,'yaxis') % NARROWBAND spectrogram
title('Spettrogramma');
xlabel('Tempo [s]')
ylabel('Frequenza [Hz]')
subplot(3,1,3)
t_formant=(1:length(a_sorted(:,1))).*0.001;
plot(t_formant,a_sorted(:,:),'b*','MarkerSize',2.5) % plot delle formanti
title('Analisi Formanti');
xlabel('Tempo [s]')
ylabel('Frequenza [Hz]')
disp('Statistiche delle Formanti')
    F1=sum(a_sorted((round(length(a_sorted(:,1))/2)-50):(round(length(a_sorted(:,1))/2)+50),1))/100
    F2=sum(a_sorted((round(length(a_sorted(:,1))/2)-50):(round(length(a_sorted(:,1))/2)+50),3))/100
    F3=sum(a_sorted((round(length(a_sorted(:,1))/2)-50):(round(length(a_sorted(:,1))/2)+50),5))/100
    
if (t<7)
    clear a_sorted; end
clear x1;
clear t_formant;
clear x;
%grid on
end
