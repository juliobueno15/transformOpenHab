
//Função que coleta dos dados do OpenHab
function GetData() {

  var url = "http://andromeda.lasdpc.icmc.usp.br:8364/rest/items";
    var result = UrlFetchApp.fetch(url);
    var jsonresult = JSON.parse(result.getContentText());
    var object = [];
   jsonresult.forEach(function(item){
      object.push(' " '+item.name +' " : "'+ item.state+'"');
   });
    var output = object.join(",");
    var string = "{" + output + "}"
    var ObjectJSON = JSON.parse(string);
    return ObjectJSON;
  
}

//Função que salva os dados no Firebase
function SaveData(){
 var baseUrl = "https://ssc0721.firebaseio.com/";
 var secret = "spCWs0Wa4jVHSqqtgXGkYzQxjBLPRA7ysrnrjvf4";
 var database = FirebaseApp.getDatabaseByUrl(baseUrl, secret);
 var data = GetData();
 var today = new Date();
    var date = today.getFullYear()+'-'+(today.getMonth()+1)+'-'+today.getDate();
    var time = today.getHours() + ":" + today.getMinutes() + ":" + today.getSeconds();
    var dateTime = date+' '+time;
 database.updateData(dateTime,data);

}