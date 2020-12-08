from django.shortcuts import render
from django.http import Http404
from . models import Algorithm

def index(request):
    all_algorithms = Algorithm.objects.all()
    context = {
        'all_algorithms' : all_algorithms,
    }
    return render(request, 'algorithms/index.html', context )
    # jesli chcemy cos ze słownika zawrzeć w template, to chodzi o klucz, czyli string w tym wypadku

def detail(request, algorithm_id):
    try:
        alg = Algorithm.objects.get(pk = algorithm_id)
    except Algorithm.DoesNotExist:
        raise Http404("Nie ma takiego algorytmu!")

    return render(request, 'algorithms/detail.html', {'algorithm' : alg, })