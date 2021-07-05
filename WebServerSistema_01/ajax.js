HTTP/1.1 200 OK
Content-Type: text/javascript
Connection: close

function enviarGiroHorizontal() {
    
 

    //enviando e tratando solicitação ajax
    var objetoRequest = new XMLHttpRequest;
    objetoRequest.onreadystatechange = function() {
        if (this.readyState == 4 && this.status == 200) {
            var stringDados = JSON.parse(this.responseText);
            document.getElementById("teta").innerHTML="Movimento horizontal : &theta; = "+stringDados.anguloHorizontal+"&deg;";

            document.getElementById("rangeGiroHorizontal").value = stringDados.anguloHorizontal;
        }
    };
    objetoRequest.open('GET','dados.json');  // Aponta o endereço do arquivo ou servidor a ser invocado
    objetoRequest.send(null); // envia a requisiçao sem repassar nenhum parâmetro

}

function enviarGiroVertical(){
    
    document.getElementById("fi").innerHTML="Movimento vertical : &phi; = "+document.getElementById("rangeGiroVertical").value+"&deg;";
}


