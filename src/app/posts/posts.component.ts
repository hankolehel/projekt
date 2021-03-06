import { Component, OnInit } from '@angular/core';
import { PostsService } from '../posts.service';

@Component({
  selector: 'app-posts',
  templateUrl: './posts.component.html',
  styleUrls: ['./posts.component.css']
})
export class PostsComponent implements OnInit {
  // instantiate posts to an empty array
  fullImagePath = '/assets/newcar.png';
  boxPath = '/assets/box.png';
  carWorkingToggle : boolean = false;

 public radarChartLabels:string[] = ['A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N'];
 public distances:any = [60,80,30];
/*
   public radarChartData:any = [
     {data: [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 60, 60], label: 'Left'},
     {data: [80, 80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 80], label: 'Middle'},
     {data: [0, 30, 30, 30, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0], label: 'Right'}

   ];
   */
      public radarChartData:any = [
        {data: [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0], label: 'Left'},
        {data: [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0], label: 'Middle'},
        {data: [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0], label: 'Right'}

      ];
   public radarChartType:string = 'radar';

  public radarChartOptions:any = {
      responsive: true,
      scale: {
              ticks: {
                  min: 0,
                  max: 70,
                  display: true
              },
              display: true
          }
    };

   public fillInDistanceArray(value:number, direction:string){
      var aux = Array<number>(10).fill(0);
      if (direction =="left"){
          aux[11] = value;
          aux[12] = value;
          aux[13] = value;
      }else{
        if (direction =="center"){
            aux[0] = value;
            aux[1] = value;
            aux[13] = value;
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

   //also sends commands to the server (car's on/off state), this gets separated at service level

   public collectStoredData(carWorkingToggle?: boolean):void{
              this.postsService.getCurrentData(carWorkingToggle).subscribe(posts => {
               console.log(posts);
               this.distances[0] = posts.left;
               this.distances[1] = posts.center;
               this.distances[2] = posts.right;

                this.rebuildChart(this.fillInDistanceArray( posts.left, "left"),
                                    this.fillInDistanceArray( posts.center, "center"),
                                      this.fillInDistanceArray( posts.right, "right"));
              });

   }

  constructor(private postsService: PostsService) {
    setInterval(()=>this.collectStoredData(), 100)
  }

   public toggleCarAction():void{
       this.carWorkingToggle = !this.carWorkingToggle;
       this.collectStoredData(this.carWorkingToggle);
   }

  ngOnInit() {
/*    this.postsService.getAllPosts().subscribe(posts => {
      this.posts = posts;
    });
*/
  }
}
