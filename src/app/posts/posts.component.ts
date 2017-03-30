import { Component, OnInit } from '@angular/core';
import { PostsService } from '../posts.service';

@Component({
  selector: 'app-posts',
  templateUrl: './posts.component.html',
  styleUrls: ['./posts.component.css']
})
export class PostsComponent implements OnInit {
  // instantiate posts to an empty array
  posts: any = [];

 public radarChartLabels:string[] = ['Eating', 'Drinking', 'Sleeping', 'Designing', 'Coding', 'Cycling', 'Running', 'Eating', 'Drinking', 'Eating', 'Drinking'];

   public radarChartData:any = [
     {data: [80, 80, 0, 0, 0, 0, 0, 0, 0, 0, 80], label: 'Left'},
     {data: [0, 0, 0, 0, 0, 0, 0, 0, 60, 60, 60], label: 'Middle'},
     {data: [0, 30, 30, 30, 0, 0, 0, 0, 0, 0, 0], label: 'Right'}

   ];
   public radarChartType:string = 'radar';

   // events
   public chartClicked(e:any):void {
     console.log(e);
   }

   public fillInDistanceArray(value:number, direction:string){
      var aux = Array<number>(10).fill(0);
      if (direction =="left"){
        aux[0] = value;
        aux[1] = value;
        aux[10] = value;
      }else{
        if (direction =="middle"){
          aux[8] = value;
          aux[9] = value;
          aux[10] = value;
        }else{
          if (direction =="right"){
            aux[1] = value;
            aux[2] = value;
            aux[3] = value;
          }
        }
      }

      return aux;
   }

   public rebuildChart(left:number[],middle:number[],right:number[]){
         let clone = JSON.parse(JSON.stringify(this.radarChartData));
         clone[0].data = left;
         clone[1].data = middle;
         clone[2].data = right;
         this.radarChartData = clone;
   }

   public randomize():void{
      this.rebuildChart(this.fillInDistanceArray( Math.round(Math.random() * 100), "left"),
                    this.fillInDistanceArray( Math.round(Math.random() * 100), "middle"),
                      this.fillInDistanceArray( Math.round(Math.random() * 100), "right"));
   }

   public chartHovered(e:any):void {
     console.log(e);
   }

  constructor(private postsService: PostsService) { }

  ngOnInit() {
    // Retrieve posts from the API
    this.postsService.getAllPosts().subscribe(posts => {
      this.posts = posts;
    });



  }
}
