void merge(int arr[],int start,int mid,int end)
{
    //ar[start:mid] e arr[mid+1:end] sono ordinati
    //posso comporli in un unico array ordinato
    //mantenendo un indice indipendente per ciascuna metà
    int n1=mid-start+1;
    int n2=end-mid;
    int L[n1-1];
    int R[n2-1];
    for(int i=0;i<n1;i++) L[i]=arr[start+i-1];
    for(int j=0;j<n2;j++) R[j]=arr[mid+j];
    int i=0;
    int j=0;
    for(int k=start;k<end;k++)
    {
        if(i<=n1)
        {
            if(j<=n2)
            {
                if(L[i]<=R[j])
                {
                    arr[k]=L[i];
                    i++;
                }else
                {
                    arr[k]=R[j];
                    j++;
                }
            }else
            {
                arr[k]=L[i];
                i++;
            }
        }else
        {
            arr[k]=R[j];
            j++;
        }
    }
}

void merge_sort(int arr[],int start,int end)
{
    //se l'array contiene almeno due elementi richiama ricorsivamente sulle due metà
    //dopo la chiamata, le due metà sono ordinate e possono essere unite con merge
    if(start<end)
    {
        int mid=(end-start)/2;
        merge_sort(arr,start,mid);
        merge_sort(arr,mid+1,end);
        merge(arr,start,mid,end);
    }
}

int main()
{
    int arr[10]={2,4,1,5,6,7,9,2,1,6};
    merge_sort(arr,0,10);
}