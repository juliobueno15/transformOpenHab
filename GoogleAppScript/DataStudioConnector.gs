var cc = DataStudioApp.createCommunityConnector();


//Função de autenticação para conectar com o DataStudio
//Nenhuma autenticação é necessária para esse projeto
function getAuthType() {
  return {
    type: "NONE"
  };
}


//Função de Configuração necessaria para o DataStudio
//Parametro dateRangeRequeried possibilita o DataStudio a mandar filtros de datas
function getConfig() {
  return {
    dateRangeRequired: true
  };
}



//Função que retornar o tipo de dados (Schema) para o Data Studio
function getSchema() {
  return {
    schema: [
      {
         name: 'date',
        label: 'Date',
        dataType: 'STRING',
        semantics: {
          conceptType: 'DIMENSION',
          semanticGroup: 'DATE',
          semanticType: 'YEAR_MONTH_DAY'
        }
      },
       {
       name: 'VasoInterno_number_temperatura',
        label: 'Temperatura Vaso Interno °C',
       dataType: 'NUMBER',
        semantics: {
          conceptType: 'METRIC'
        }
      },
       {
       name: 'VasoInterno_number_umidade',
        label: 'Umidade Vaso Interno %',
       dataType: 'NUMBER',
        semantics: {
          conceptType: 'METRIC'
        }
      },
       {
       name: 'VasoExterno_number_luminosidade',
        label: 'Luminosidade %',
        dataType: 'NUMBER',
        semantics: {
          conceptType: 'METRIC'
        }
      },
       {
       name: 'VasoExterno_number_temperatura',
        label: 'Vaso Temperatura °C',
        dataType: 'NUMBER',
        semantics: {
          conceptType: 'METRIC'
        }
      },
       {
       name: 'VasoExterno_number_umidade',
        label: 'Umidade %',
        dataType: 'NUMBER',
        semantics: {
          conceptType: 'METRIC'
        }
      }
      
      
    ]
  };
}



//Função que retornar os dados para o DataStudio
function getData(request) {
  var dataSchema = [];
  var fixedSchema = getSchema().schema;
  
  // Gerando o Schema para ser enviado para o Data Studio
  request.fields.forEach(function(field) {
    for (var i = 0; i < fixedSchema.length; i++) {
      if (fixedSchema[i].name == field.name) {
        dataSchema.push(fixedSchema[i]);
        break;
      }
    }
  });
  
  
 //Capturando dados do Firebase e adequando para o formato necessário para o Data Studio 
  var data =formatData(GetData(),dataSchema,request);
 
  
  return {
    schema: dataSchema,
    rows: data
  };
  
}



//Função que formata os dados para serem enviados ao Data Studio 
function formatData(object , dataSchema, request)
{
  var dados = [];
  
    //Varre a coleção de dados
    for(var item in object){
    var values = [];
    //Recupera as datas do filtro
    var endDate = new Date(request.dateRange.endDate);
    var startDate = new Date(request.dateRange.startDate);
    var current_date = new Date(item);
    //Verifica se a send date está dentro do periodo pedido pelo DataStudio
    if(current_date.getTime()<=endDate.getTime() && current_date.getTime()>=startDate.getTime()){
    //Função que atrela os dados aos campos
      dataSchema.forEach(function(field) {
          switch (field.name) {
        case 'date':
          values.push(item);
          break;
         case 'VasoInterno_number_temperatura':
          values.push(object[item][' VasoInterno_number_temperatura ']);
          break;
         case 'VasoInterno_number_umidade':
          values.push(parseInt(object[item][' VasoInterno_number_umidade '])/100);
          break;
         case 'VasoExterno_number_luminosidade':
          values.push(parseInt(object[item][' VasoExterno_number_luminosidade '])/100);
          break;    
         case 'VasoExterno_number_temperatura':
          values.push(object[item][' VasoExterno_number_temperatura ']);
          break;
         case 'VasoExterno_number_umidade':
          values.push(parseInt(object[item][' VasoExterno_number_umidade '])/100);
          break;
         default:
          values.push('');
         }
        });
     dados.push({values : values});
     }
    }  
   
    
  
  return dados;
}

//Função que retorna como ADMIN para o DATAStudio
function isAdminUser() {
  return true;
}
  